include ../make_setting

all:
	cd Cell && make
	cd Node && make
	cd NonUniformCell && make
ifneq (, $(findstring -DCUTLIB_OCTRE, $(DEFINES)))
	cd Octree && make
endif

clean:
	cd Cell && make clean
	cd Node && make clean
	cd NonUniformCell && make clean
	cd Octree && make clean

dist_clean:
	cd Cell && make dist_clean
	cd Node && make dist_clean
	cd NonUniformCell && make dist_clean
	cd Octree && make dist_clean

