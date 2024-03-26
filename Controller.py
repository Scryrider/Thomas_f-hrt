from pyPS4Controller.controller import Controller
from pyPS4Controller.event_mapping.DefaultMapping import DefaultMapping

def map_value(x):
    x_min, x_max = 0, 33000
    y_min, y_max = 0, 255
    y = (x - x_min) * (y_max - y_min) / (x_max - x_min) + y_min
    return int(y)  # Konvertierung zu einem ganzzahligen Wert


#print(map_value(x))

class MyController(Controller):

    def __init__(self, **kwargs):
        Controller.__init__(self, **kwargs)

    def on_R3_right(self,overflow):
       print(map_value(overflow))
    
    def on_R3_left(self,overflow):
       print(map_value(overflow*(-1)))

    def on_L3_down(self,overflow):
       print(map_value(overflow))

    def on_L3_up(self,overflow):
       print(map_value(overflow*(-1)))




class MyEventDefinition(DefaultMapping):

    def __init__(self, **kwargs):
        DefaultMapping.__init__(self, **kwargs)
    
    def on_R3_right(self,overflow):
       print(print(map_value(overflow)))

    def on_R3_left(self):
       print(print(map_value(overflow*(-1))))



controller = MyController(interface="/dev/input/js0", connecting_using_ds4drv=False)
controller.listen()