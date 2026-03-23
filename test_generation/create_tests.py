# we need to create certain test to check if our orderbook is working or not
# our test case if going to be relative simple, we are going to have an 
# orderid, timestamp, event_type (add/cancel), direction(buy/sell), price (optional, i.e. not imp in market order),
#  quantity, order type (Market order currently), ticker
# TODO be added later order type (Market order, Limit order, stop loss order )
# TODO currently prices are rounded off to 2 decimal points, can be upgraded to tick based pricing

import random
import uuid
import time
import json
import pprint

def clamp(n, min, max):
    if n < min:
        return min
    elif n > max:
        return max
    else:
        return n


def create_mkt_price_generator():
    '''
    This is a generator to yield the price at any step, it utilised a mean reversion  

    '''
    min_price, max_price = 8 , 10
    avg_price = 9
    initial_price = 9
    k = 0.05

    # price = price + noise + k * (mean - price)
    while True:
        price = initial_price + random.uniform(-0.1, 0.1) + k*(avg_price-initial_price)
        price = clamp(price, min_price, max_price)
        initial_price = price
        yield price

def buy_sell_price(price_gen, min_spread=0.01):
        mid = next(price_gen)
        spread = max(random.uniform(0.02, 0.1), min_spread)
        buy_price = round(mid - spread/2, 2)
        sell_price = round(mid + spread/2, 2)
        # defensive correction
        if buy_price >= sell_price:
            sell_price = round(buy_price + min_spread, 2)
        return buy_price, sell_price


def validate_order_event(ev):
    """Validate generated order event invariants.

    Raises ValueError on invalid events.
    """
    if not isinstance(ev, dict):
        raise ValueError("event must be a dict")
    et = ev.get('event_type')
    if et == 'ADD':
        # direction
        if ev.get('direction') not in ('BUY', 'SELL'):
            raise ValueError(f"Invalid direction: {ev.get('direction')}")
        # quantity
        qty = ev.get('quantity')
        if not isinstance(qty, int) or qty <= 0:
            raise ValueError(f"Invalid quantity: {qty}")
        # price (if present)
        if 'price' in ev:
            price = ev.get('price')
            if not (isinstance(price, (int, float)) and price > 0):
                raise ValueError(f"Invalid price: {price}")
    elif et == 'CANCEL':
        if not ev.get('target_orderid'):
            raise ValueError("CANCEL missing target_orderid")
    else:
        raise ValueError(f"Unknown event_type: {et}")

def generate_limit_price(price_generator, order_type):
        """Generate an order event dict. """
        buy_price, sell_price = buy_sell_price(price_generator)
        orderid = uuid.uuid4()
        timestamp = time.time_ns()
        direction =  random.choice(['BUY', 'SELL'])
        quantity = random.randint(1, 10)
        ticker = 'ABC'

        # For market orders we don't include price; event_type for adds is 'ADD'
        if order_type == 'MARKET':
            return {
                'orderid': str(orderid),
                'timestamp': timestamp,
                'event_type': 'ADD',
                'direction': direction,
                'quantity': quantity,
                'order_type': order_type,
                'ticker': ticker,
            }

        aggressive = random.random() < 0.3   # 30%
        if aggressive:
            delta = random.uniform(0.01, 0.05)
            if direction == 'BUY':
                price = round(sell_price + delta, 2)   # cross ask
            else:
                price = round(buy_price - delta, 2)    # cross bid
        else:
            price = buy_price if direction == 'BUY' else sell_price

        return {
            'orderid': str(orderid),
            'timestamp': timestamp,
            'event_type': 'ADD',
            'direction': direction,
            'price': price,
            'quantity': quantity,
            'order_type': order_type,
            'ticker': ticker,
        }

def create_test(no_of_test):
    '''
    These test cases will write 1000 mkt order + limit order
    The limit order will help to reach the intial step of price discovery
    we will move price in a 5 % std dev, with 1s on milli second scale around $10 and random quantity from (1 to 1000)
    '''
    price_generator = create_mkt_price_generator()

    # collect generated events and active orders so cancellations can reference real orders
    events = []
    active_orders = []

    # seed with some LIMIT orders
    initial_book_making_test = no_of_test//2
    for i in range(initial_book_making_test):
        order_type = 'LIMIT'
        ev = generate_limit_price(price_generator, order_type)
        validate_order_event(ev)
        events.append(ev)
        # only non-market adds stay active
        if ev.get('event_type') == 'ADD' and ev.get('order_type') != 'MARKET':
            active_orders.append(ev['orderid'])
    
    final_book_making_test = no_of_test-initial_book_making_test
    for i in range(final_book_making_test):
        order_type = random.choices(['LIMIT', 'MARKET', 'CANCEL'], weights=[70, 20, 10])[0]
        if order_type == 'LIMIT':
            ev = generate_limit_price(price_generator, order_type)
            validate_order_event(ev)
            events.append(ev)
            active_orders.append(ev['orderid'])
        elif order_type == 'MARKET':
            # Market orders are going to be executed immediately, so it is not kept in active state
            ev = generate_limit_price(price_generator, order_type)
            validate_order_event(ev)
            events.append(ev)
        else:
            # CANCEL: pick a random active order to cancel (if any)
            if active_orders:
                target = random.choice(active_orders)
                timestamp = time.time_ns()
                cancel_ev = {
                    'event_type': 'CANCEL',
                    'timestamp': timestamp,
                    'target_orderid': target,
                }
                events.append(cancel_ev)
                # remove from active list to avoid double-cancelling
                try:
                    active_orders.remove(target)
                except ValueError:
                    pass
            else:
                # no active orders to cancel; generate a new limit instead
                ev = generate_limit_price(price_generator, 'LIMIT')
                events.append(ev)
                active_orders.append(ev['orderid'])
    #pprint.pprint(events)

    # persist generated events so tests can consume them
    out_path = 'test_generation/generated_events.json'
    with open(out_path, 'w') as f:
        json.dump(events, f, indent=2)
    print(f"Wrote {len(events)} events to {out_path}")




if __name__ == "__main__":
    no_of_test = 1000
    create_test(no_of_test)
