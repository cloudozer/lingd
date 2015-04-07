# LING Dom0 service

lingd allows to boot LING apps via network, by exposing single directory with LING image and config files to client hosts.

# Dependencies
 * gcc and Erlang/OTP 17.0 on master host
 * Xen xl toolstack on client hosts

# Installation on master host
Clone repo and do the make:
```
	git clone https://github.com/cloudozer/lingd.git
	cd lingd
	make
	sudo make install
```
Last step will create an /io dir on your master host - it is a central storage for all LING images and configs.

Now install some app in it. Currently the only tested app is LINCX. 
```
	git clone https://github.com/cloudozer/lincx.git
	cd lincx
	./rebar get-deps
	./rebar co
	cp lincx.yml.sample lincx.yml # default config will be fine, we need only img file from railing
	./railing image
	sudo ./scripts/lingd-install
```
This will put required LINCX files to /io dir.

Now lets configure LINCX for some client host, e.g. host peer1. Create subdir for it and host specific LINCX config:
```
	mkdir -p  /io/peer1/lincx1
    nano /io/peer1/lincx1/lincx1.yml
```
Fill yml file with something like that:
```
app: lincx

queues:
  - {id: 1, min: 0, max: 100}
  - {id: 2, min: 0, max: 200}

ports:
  - {id: 1, bridge: xenbr1}
  - {id: 2, bridge: xenbr2}
  - {id: 3, bridge: xenbr3}

controllers:
  - 192.168.3.30:6653
  - 10.10.1.1:6633

listen: 0.0.0.0:6653

memory: 512
```
(assuming that peer1 has all xenbr's set up).

The last thing needed is to generate secret key for lingd. Return to lingd dir and do the following:
```
	./lingd --generate-key=lingd.key
```
Ok, master host is ready to serve. Launch lingd on it in server mode:
```
	./lingd -s
```
Copy lingd binary and lingd.key to your client host and launch lingd from it in boot mode:
```
	./lingd -b 192.168.3.254:peer1
```
If everithing went smoothly you can check launched LINCX by executing on client host:
```
	xl console lincx1
```
