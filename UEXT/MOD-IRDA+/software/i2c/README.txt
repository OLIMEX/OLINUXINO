This package control I2C bus.


Example
=======

Typical usage::
    
    #!/usr/bin/env python

    import I2C

    #init module
    I2C.init(<bus number>)

    #Start communcitaion with device
    I2C.open(<device address>)

    #Send byte of data to the device
    I2C.write([<data1>, <data2>, <data2>, ...])

    #Read data from the device
    data = I2C.read(<bytes to read>)
 
    #Close communication with device
    I2C.close(bus)
    
    


