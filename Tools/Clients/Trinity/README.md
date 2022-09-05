# Trinity
> by KeyZiro

Yup, you read that right. I absolutely hate anyone selling cheat codes in any way, so hopefully this will be a nice lesson for you.
###### ~~$10 is too fucking much by the way~~  
These files are not cracked yet, however you are free to attempt to crack them yourself. The RPX's function names are obfuscated.

## How to Use

SDL-Install__.rpx can be loaded on the Homebrew Launcher and launched that way, without having to go to a website to do so.
However, it will still be pointed towards Ziro's servers (51.91.56.16). Since his servers are HTTP instead of HTTPS, you could easily spoof them to your own server and run it that way. I personally could not find out how to use a DNS to reroute an IP address to my own IP address, however I will gladly accept help if anyone is offering.

## Requests

Once you visit nonhost.ml, the giant "Start WiiU Non-Host mods" link will take you to http://51.91.56.16/bo2/index-hax.php, which will run an exploit to run the following file: http://51.91.56.16/payload.elf. payload.elf will then run another file called http://51.91.56.16/launcher.elf, which still start downloading SDL-Install__.rpx.

After the download is finished, SDL-Install__.rpx will be run, however the previous requests can be skipped if you run the rpx directly from the Homebrew Launcher.

The RPX then requests the following URLs:

> All of these requests must have these as the only headers: `Accept: */*`, and `Host: 51.91.56.16`  
> ---------------------------------------------------  
> http://51.91.56.16/online (content-type is `application/octet-stream`, returns `1\n`)  
> http://51.91.56.16/version (content-type is `application/octet-stream`, returns `v2.5\n`)  
> http://51.91.56.16/launcher/2vl_logo.png (content-type is `image/png`, Vulcan Logo)  
> http://51.91.56.16/launcher/2ph_logo.png (content-type is `image/png`, Phantom Logo)  
> http://51.91.56.16/launcher/2tr_logo.png (content-type is `image/png`, Trinity Logo)  
> ---------------------------------------------------  
> The following requests require this header to be included: `SID: 1234`  
> Otherwise, it will return with a simple "YA BITCH". He knew LOL  
> http://51.91.56.16/stats.php?type=website (content-type is `text/html; charset=UTF-8`, returns `<br/>website`)  
> http://51.91.56.16/stats.php?type=trinity (content-type is `text/html; charset=UTF-8`, returns `<br/>trinity`)  
> ---------------------------------------------------  
> After pressing "Launch" on Trinity, the following file is downloaded and run (I'm still not too sure how the file is run, but disassembling the RPX could lead to clues):  
> http://51.91.56.16/adilmc.php (content-type is `application/octet-stream`, returns the adilmc file in the repository)  

Once Trinity has been launched, Minecraft: Wii U is automatically started up.

However, I noticed a request from IP 255.255.255.0, and it's data reads as the following:
```
==================
GameHackingHelper by NexoCube
We're started ...
```
This proves that Trinity runs under GameHackingHelper, which when searched on google, leads to this github link: https://github.com/Rambo6Glaz/WiiUGameHacking

Interestingly, there's one more check while Minecraft is running, so that means the full version of Trinity is downloaded, not just the trial version. It just does a simple request to see if you bought it, and if you haven't, it displays a warning message on the bottom. (I am unaware of the url requested, however it can likely be easily spoofed, the data sent is unclear however)

So yeah, have fun with this.
