from smbus import SMBus

clientAddr = 0x08
bus = SMBus(1)

def i2cWrite(msg):
  for c in msg:
    bus.write_byte(clientAddr, ord(c))
  return -1

def main():
  print("Send message to Arduino")
  while True:
    msg = input("$> ")
    print("...")
    i2cWrite(msg)

if __name__ == "__main__":
  main()


"""from smbus import SMBus

addr = 0x8 # bus address
bus = SMBus(1) # indicates /dev/ic2-1

numb = 1

print ("Enter 1 for ON or 0 for OFF")
while numb == 1:

    ledstate = input(">>>>   ")

    if ledstate == "1":
        bus.write_byte(addr, 0x1) # switch it on
    elif ledstate == "0":
        bus.write_byte(addr, 0x0) # switch it on
    else:
        numb = 0
"""