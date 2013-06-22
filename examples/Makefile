include ../make_setting

all:
	cd Cell && make
	cd Node && make
	cd Cell_Normal && make
	cd Node_Normal && make
ifneq (, $(findstring -DCUTLIB_OCTRE, $(DEFINES)))
	cd Octree && make
endif

clean:
	cd Cell && make clean
	cd Node && make clean
	cd Cell_Normal && make clean
	cd Node_Normal && make clean
	cd Octree && make clean

dist_clean:
	cd Cell && make dist_clean
	cd Node && make dist_clean
	cd Cell_Normal && make dist_clean
	cd Node_Normal && make dist_clean
	cd Octree && make dist_clean

