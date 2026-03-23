# we need to create certain test to check if our orderbook is working or not
# our test case if going to be relative simple, we are going to have an 
# orderid, timestamp, event_type (add/cancel), direction(buy/sell), price (optional, i.e. not imp in market order),
#  quantity, order type (Market order currently), ticker
# TODO be added later order type (Market order, Limit order, stop loss order )
# TODO currently prices are rounded off to 2 decimal points, can be upgraded to tick based pricing

import random
import uuid
import time

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

def buy_sell_price(price_gen):
        mid = next(price_gen)
        spread = random.uniform(0.02,0.1)
        buy_price = round(mid - spread ,2)
        sell_price = round(mid + spread,2)
        return buy_price, sell_price

def create_test():
    '''
    These test cases will write 1000 mkt order + limit order
    The limit order will help to reach the intial step of price discovery
    we will move price in a 5 % std dev, with 1s on milli second scale around $10 and random quantity from (1 to 1000)
    '''
    price_generator = create_mkt_price_generator()

    # create 200 limit order

    for i in range(20):
        buy_price, sell_price = buy_sell_price(price_generator)
        orderid = uuid.uuid4()
        timestamp = int(time.time() * 10000)
        event_type = random.choice(['ADD']) # we are adding only to fill the orderbook
        direction =  random.choice(['BUY', 'SELL'])
        price = buy_price if direction=='BUY' else sell_price
        quantity = random.randint(1, 10)
        order_type = 'LIMIT'
        ticker = 'ABC'
        print(orderid, timestamp, event_type, direction, price, quantity, order_type, ticker)


if __name__ == "__main__":
    create_test()
