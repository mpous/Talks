# Kerberos.io running on balena

In this project, we will deploy Kerberos on balena to set up a security camera system. Since it uses open source software and balena, it’s much more private than other cloud-focused solutions, too. None of your videos are being stored anywhere other than your edge device.

[![](https://balena.io/deploy.svg)](https://dashboard.balena-cloud.com/deploy?repoUrl=https://github.com/mpous/kerberos-balena)

<img width="1554" alt="Kerberos running on balena" src="https://user-images.githubusercontent.com/173156/163893446-41a67e5e-5bd8-40a5-b1c7-05c3db4ff563.png">

## Hardware required

* A Raspberry Pi 2/3/3b+/4 or Nvidia Jetson Nano. It also works with x86 devices.
* A 32GB+ SD Card
* A USB webcam (be aware of its resolution specs-- you’ll need this info for the build)
* Power supply

## Software required

* A free balenaCloud account (your first ten devices are free and fully-featured)
* [balenaEtcher](https://balena.io/etcher) to burn OS image to SD cards
* (optional, and will add time to your build) [balenaCLI](https://www.balena.io/docs/reference/balena-cli/) if you're an advanced user who wants to hack on your devices, work locally, etc.


## Deploy the project 


Kerberos.io works on armv7 (Raspberry Pi 2/3/3b+ and others), aarch64 (Nvidia Jetson, Raspberry Pi 4 and others) and AMD64 (laptops and desktops). Selecting your type of device will make the balena builders create the correct image for you and deploy it to your application.

