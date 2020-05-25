# sys-http

This is a sysmodule that allows communicating with the Nintendo Switch using http. This sysmodule brings a few advantages that don't currently exist:

- HTTP is a standardized protocol with well defined uses
- Most languages have at least one http library, so there's not a need to write a wrapper around raw TCP
- Web browsers can use HTTP, but not raw TCP (without disabling security rules)

Potential future use cases:

- Read Pokemon Sword/Shield den seeds directly into @Admiral-Fish's [RaidFinder](https://github.com/Admiral-Fish/RaidFinder) or @leanny's [Seed Checker](http://leanny.github.io/seedchecker)
- Display Animal Crossing turnip prices in a mobile app
- Show updating game values during a Twitch live stream
- Setup a [PKHeX REST API](https://github.com/zaksabeast/PKHeX-Lambda) and live edit Pokemon from a website
- Create a Discord bot that allows users to interact with the console

## Installing

1. Download the zip from the latest releases
1. Copy the zip file contents to your switch SD card at `/atmosphere/contents`

It's recommended to use something like [Kosmos Toolbox/Hekate Toolbox](https://github.com/WerWolv/Hekate-Toolbox) and disable the sysmodule when it's not in use.

## Developing frontend applications

These are helpful pieces of information when building frontend applications that use sys-http.

### Connecting

- sys-http uses port 8080
- https is not currently used

### Available routes

All number query parameters will be in hex format for consistency.

These are the currently available routes:

- `/` - Health check to see if the sysmodule is running
- `/refreshMetadata` - Refreshes the game metadata used to power other routes
- `/titleId` - Returns the title Id of the game
- `/readHeap?offset=<hex>&size=<hex>` - Read values from the heap

## Credits

Thanks to the following groups, individuals, and contributors:

- ReSwitched for [Atmosphere-libs](https://github.com/Atmosphere-NX/Atmosphere-libs)
- Switchbrew for [libnx](https://github.com/switchbrew/libnx)
- yhirose for [cpp-httplib](https://github.com/yhirose/cpp-httplib/)
