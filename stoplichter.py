#!/usr/bin/env python3

import time

import sys
sys.path.append('lib')
from strip import Effect, Strip, Strip2D


# TODO: strip (pun intended) the effect from this class: only use Strip
# self.strip = Strip(150, addr)

off = [ 0, 0, 0 ]
red = [ 255, 0, 0 ]
yellow = [ 255, 96, 0 ]
green = [ 0, 255, 0 ]
# Odd colors
blue = [0, 0, 255]
pink = [255, 0, 255]

# Lights go from bottom to top
none = [ off, off, off ]
traffic_all = [ green, yellow, red ]

traffic_stop = [ off, off, red ]
traffic_prepare = [ off, off, red ] # choose between yellow-red or just red
traffic_go = [ green, off, off ]
traffic_caution = [off, yellow, off ]

"""
bridge_closed_indefinitely = [ red, off, red ]
bridge_closed = [ off, off, red ]
bridge_prepare = [ off, green, red ]
bridge_go = [ off, green, off ]
bridge_caution = [ off, off, red ]

# TODO: automated (orange) with 2-way boats
bridge = [
            [ bridge_closed, 5, sema_go ] ,
            [ bridge_prepare, 3.5, sema_prepare ],
            [ bridge_go, 5, sema_go ],
            [ bridge_caution, 3.5, sema_prepare ]
          ]

# Yellow light duration:
# @80km/h → 5s
# @70km/h → 4.5s
# @50km/h → 3.5s
# overlap stop time, stop+time, prepare+time, go+time, caution+time
traffic = [
            [ traffic_stop, 0, sema_go ],           # intersection release time (all red)
            [ traffic_prepare, 1.5, sema_prepare ], # prepare time (see next NOTE)
            [ go, 5, sema_go ],                     # green time
            [ traffic_caution, 3.5, sema_prepare ]  # orange time
          ]
# NOTE: prepare time is calculated as a remainder of caution+stop time
traffic[1][1] = max( traffic[0][1] + traffic[3][1] - traffic[1][1], 0 )
"""

oddCaution = [off, blue, off]
hey = [off, off, pink]

class Stoplichter(Effect):
  def __init__(self, strip2D, colors = None):
    super(Stoplichter, self).__init__(strip2D)
    self.strip2D.strip.clear()
    self.colors = colors or [ red, yellow, green, off ]


  def run(self, runtime = None):
    if runtime is None:
      if hasattr( sys, "maxint" ): # Python 2
        runtime = sys.maxint
      elif hasattr( sys, "maxsize" ): # Python 3
        runtime = sys.maxsize

    print( self.strip2D.strip.length )
    now = time.time()
    offset = 0
    while (not self.quit) and ((time.time() - now) < runtime):

      self.strip2D.strip.set(144, red)
      self.strip2D.strip.set(145, yellow)
      self.strip2D.strip.set(146, green)

      self.strip2D.send()

      time.sleep(0.20)

    self.quit = False


"""
./stoplichter.py addr=192.168.1.255
./stoplichter.py [GERMANr] 'addr=[("192.168.1.255", ), ("localhost", 7000)]'
"""

strip = Strip(150)

# Channel fade
def channelFade( c1, c2, r ):
  return int(c1 * (1 - r)) + c2 * r

def colorFade( c1, c2, r ):
  return [channelFade(c1[0],c2[0],r),channelFade(c1[1],c2[1],r),channelFade(c1[2],c2[2],r)]

def lightFade( l1, l2, r ):
  return [colorFade(l1[0],l2[0],r),colorFade(l1[1],l2[1],r),colorFade(l1[2],l2[2],r)]


def orangeBlink( amount ):
  while amount > 0:
    strip.setm(144, traffic_caution)
    strip.send()
    time.sleep( 1.1 )
    strip.setm(144, none)
    strip.send()
    time.sleep( 1.1 )
    amount -= 1

def oddOrangeBlink( amount, fluid = False ):
  current = 0
  frames = fluid and 10 or 1

  while current < amount:
    # TODO: set framerate with time interval current + time fade amount
    for frame in range(frames):
      strip.setm(144, lightFade(traffic_caution, oddCaution, (current+(frame/frames)) / amount))
      strip.send()
      time.sleep( 0.11 * (11-frames) )
    strip.setm(144, none)
    strip.send()
    time.sleep( 1.1 )
    current += 1

def normalMode():
  # prepare
  strip.setm(144, traffic_prepare)
  strip.send()
  time.sleep( 2.5 )

  # go
  strip.setm(144, traffic_go)
  strip.send()
  time.sleep( 5 )

  # caution
  strip.setm(144, traffic_caution)
  strip.send()
  time.sleep( 3.5 )

  # stop
  strip.setm(144, traffic_stop)
  strip.send()
  time.sleep( 6 )

def specialMode():
  normalMode()
  frames = 20
  for frame in range(frames):
    strip.setm(144, lightFade(traffic_stop, hey, frame / frames))
    strip.send()
    time.sleep( 0.1 )



if __name__ == "__main__":

  while True:
    orangeBlink(2)
    oddOrangeBlink(4, False)
    normalMode()
    specialMode()
    time.sleep( 2 )

  # TODO: spaceAPI
  """
    space closed + movement: orange blink
    space open: green
    space closed+devices on: orange (check every 2 seconds?)
    space closed+devices off: red (1 minute)
    problem: top+bottom flash orange
    break-in: top+bottom flash red+blue

  """