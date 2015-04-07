.PHONY: default bootling install
default: lingd bootling

CFLAGS := -O2 -flto

LINGD := \
	src/lingd.o \
	src/lingd_secret.o \
	src/lingd_io.o

DIOD := \
	deps/diod/libnpclient/fsys.o \
	deps/diod/libnpclient/mtfsys.o \
	deps/diod/libnpclient/mount.o \
	deps/diod/libnpclient/pool.o \
	deps/diod/libnpclient/fid.o \
	deps/diod/libnpclient/walk.o \
	deps/diod/libnpclient/open.o \
	deps/diod/libnpclient/write.o \
	deps/diod/libnpfs/np.o \
	deps/diod/libnpfs/npstring.o \
	deps/diod/libnpfs/conn.o \
	deps/diod/libnpfs/fidpool.o \
	deps/diod/libnpfs/fcall.o \
	deps/diod/libnpfs/ctl.o \
	deps/diod/libnpfs/error.o \
	deps/diod/libnpfs/user.o \
	deps/diod/libnpfs/trans.o \
	deps/diod/libnpfs/fmt.o \
	deps/diod/libnpfs/srv.o \
	deps/diod/libnpfs/fdtrans.o \
	deps/diod/liblsd/list.o \
	deps/diod/liblsd/hostlist.o \
	deps/diod/liblsd/hash.o \
	deps/diod/libdiod/diod_auth.o \
	deps/diod/libdiod/diod_conf.o \
	deps/diod/libdiod/diod_sock.o \
	deps/diod/libdiod/diod_log.o \
	deps/diod/diod/ops.o \
	deps/diod/diod/fid.o \
	deps/diod/diod/ioctx.o \
	deps/diod/diod/xattr.o \
	deps/diod/diod/exp.o

MUNGE := \
	deps/munge/src/munged/crypto.o \
	deps/munge/src/munged/timer.o \
	deps/munge/src/munged/path.o \
	deps/munge/src/munged/md.o \
	deps/munge/src/munged/random.o \
	deps/munge/src/munged/auth_recv.o \
	deps/munge/src/munged/zip.o \
	deps/munge/src/munged/conf.o \
	deps/munge/src/munged/mac.o \
	deps/munge/src/munged/cipher.o \
	deps/munge/src/munged/cred.o \
	deps/munge/src/munged/base64.o \
	deps/munge/src/munged/enc.o \
	deps/munge/src/munged/dec.o \
	deps/munge/src/munged/replay.o \
	deps/munge/src/libmissing/strlcpy.o \
	deps/munge/src/libcommon/m_msg.o \
	deps/munge/src/libcommon/fd.o \
	deps/munge/src/libcommon/str.o \
	deps/munge/src/libcommon/log.o \
	deps/munge/src/libcommon/license.o \
	deps/munge/src/libcommon/version.o \
	deps/munge/src/libmunge/auth_send.o \
	deps/munge/src/libmunge/m_msg_client.o \
	deps/munge/src/libmunge/ctx.o \
	deps/munge/src/libmunge/strerror.o \
	deps/munge/src/libmunge/decode.o \
	deps/munge/src/libmunge/encode.o

BRIDGE := \
	deps/libbridge/libbridge_devif.o \
	deps/libbridge/libbridge_if.o \
	deps/libbridge/libbridge_init.o \
	deps/libbridge/libbridge_misc.o

$(LINGD): %.o: %.c
	gcc -c $< \
		-Ideps/diod/libnpfs \
		-Ideps/munge/src/munged \
		-Ideps/diod/diod \
		-Ideps/diod/libdiod \
		-Ideps/diod/libnpclient \
		-Ideps/diod/liblsd \
		-Ideps/libbridge \
		$(CFLAGS) -std=gnu11 -D_GNU_SOURCE -o $@
		

$(DIOD): %.o: %.c
	gcc -c $< \
		-Ideps/diod/config \
		-Ideps/munge/src/munged \
		-Ideps/munge/src/libmunge \
		-Ideps/diod/libnpfs \
		-Ideps/diod/libnpclient \
		-Ideps/diod/libdiod \
		-Ideps/diod/liblsd \
		$(CFLAGS) -DHAVE_CONFIG_H -o $@

$(MUNGE): %.o: %.c
	gcc -c $< \
		-Ideps/munge/config \
		-Ideps/munge/src/libmissing \
		-Ideps/munge/src/libmunge \
		-Ideps/munge/src/libcommon \
		$(CFLAGS) -DHAVE_CONFIG_H -o $@

$(BRIDGE): %.o: %.c
	gcc -c $< \
		$(CFLAGS) -o $@

lingd: $(LINGD) $(DIOD) $(MUNGE) $(BRIDGE)
	gcc $^ $(CFLAGS) -lpthread -lcrypto -o $@
	strip $@

bootling:
	$(MAKE) -C bootling

install: lingd bootling
	install -D bootling/bootling.img /io/bootling.img

clean:
	find . -name '*.o' -delete
	rm -f lingd
	$(MAKE) -C bootling clean