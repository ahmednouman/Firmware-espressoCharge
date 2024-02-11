import time
import usb.core

class CP2104Controller:
    def __init__(self, vid, pid):
        self.vid = vid
        self.pid = pid
        self.dev = None
        self.initialize()

    def initialize(self):
        self.dev = usb.core.find(idVendor=self.vid, idProduct=self.pid)
        if not self.dev:
            raise ValueError("CP2104 was not found")
        print("Found CP2104")

    def send_control_transfer(self, reqType, bReq, wVal, wIndex):
        self.dev.ctrl_transfer(reqType, bReq, wVal, wIndex, [])

    def set_boot_pin(self, val=0):
        reqType = 0x41
        bReq = 0xFF
        wVal = 0x37E1
        if (val == 0):
            wIndex = 0x40  
        else:
            wIndex = 0x40 | (0x40 << 8)
        self.send_control_transfer(reqType, bReq, wVal, wIndex)

    def set_reset_pin(self, val=0):
        reqType = 0x41
        bReq = 0xFF
        wVal = 0x37E1
        wIndex = 0x20 if val == 0 else 0x20 | (0x20 << 8)
        self.send_control_transfer(reqType, bReq, wVal, wIndex)

def main():
    PID = 0xea60
    VID = 0x10c4

    controller = CP2104Controller(VID, PID)

    try:
        controller.set_boot_pin(1)
        controller.set_reset_pin(1)
        # while True:
        #     controller.set_boot_pin(1)
        #     time.sleep(5)

        #     controller.set_boot_pin(0)
        #     time.sleep(5)
    except KeyboardInterrupt:
        pass
    finally:
        pass

if __name__ == "__main__":
    main()
