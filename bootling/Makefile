bootling.img: rebar src/bootling.erl
	./rebar co && \
	./railing image -lcrypto -lcompiler -m 64

rebar:
	curl -LO https://github.com/rebar/rebar/releases/download/2.5.1/rebar && \
	chmod +x rebar && \
	./rebar get-deps

LINGVER=0.3.2
railing:
	curl -LO https://github.com/cloudozer/ling/releases/download/v$(LINGVER)/ling-$(LINGVER).tar.gz && \
	tar -xf ling-$(LINGVER).tar.gz && \
	mv ling-$(LINGVER)/railing . && \
	rm -rf ling-$(LINGVER) ling-$(LINGVER).tar.gz

clean:
	rm -rf .railing bootling.img ebin rebar .rebar deps domain_config

.PHONY: clean
