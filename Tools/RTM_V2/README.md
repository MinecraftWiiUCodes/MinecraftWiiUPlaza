# Minecraft-WiiU-Python-Mod_Injector

As the title states this is a mod injector for Minecraft: Wii U edition.

This is just a simple tkinter GUI to send Memory Write codes to your game, these codes include things like Fly and Kill Aura, etc.

## Setup:

To begin with open IP.py and replace the existing Wii U IPv4 with your OWN or else it will not work. This is because IP.py assigns the
variable IP_Addr with the specific IP set which the GUI will then use to connect to that Wii U.     For example the line in the GUI 

```python
tcp = TCPGecko(IP_Addr)
```
Takes the IP_Addr variable from the IP.py file to be able to connect.

### The following modules are required!

Tkinter


### To Run:

After your IP is set in the IP.py file simply run KorozinRMTv2.py like any other .py file.

### Images

#### to run example:

![to run](https://user-images.githubusercontent.com/90534409/161388765-49d96f64-fece-46e2-a31a-2430fe6a9e18.png)

#### Running program

![Running](https://user-images.githubusercontent.com/90534409/161388777-701df958-304b-4dda-b1b0-a4759632ea4b.png)

#### Example of changing IP

![IP-select](https://user-images.githubusercontent.com/90534409/161388789-79b1fdef-021d-492d-9d41-2af428c7f692.png)

# Changelogs:

### April 2nd:  Added tabs for seperate menus instead of opening new sub menus. Hopefully this makes it more seamless.

#### Tab 1

![tab 1](https://user-images.githubusercontent.com/90534409/161439174-96f711a0-401a-4a8c-b870-69c2ba1f6d78.png)

#### Tab 2

![tab 2](https://user-images.githubusercontent.com/90534409/161439190-5304afa4-1562-44d2-b65d-29a236b69588.png)


### April 3rd: Added 10 new codes to make the tool more useful / fun.

#### Image of new added codes

![New codes](https://user-images.githubusercontent.com/90534409/161441562-608f8fe0-dfa1-4825-939e-af4e8444fe44.png)
