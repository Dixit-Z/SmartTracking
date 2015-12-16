CXX           = g++
CXXFLAGS      = -O2 -Wall -D__STDC_CONSTANT_MACROS -std=c++11
LIBS          = -lm                     \
                -lpthread               \
                -lavutil                \
                -lavformat              \
                -lavcodec               \
                -lswscale               \
                -lopencv_calib3d        \
                -lopencv_core           \
                -lopencv_features2d     \
                -lopencv_flann          \
                -lopencv_highgui        \
                -lopencv_imgproc        \
                -lopencv_ml             \
                -lopencv_objdetect      \
                -lopencv_photo          \
                -lopencv_stitching      \
                -lopencv_superres       \
                -lopencv_video          \
                -lopencv_videostab	\
                `pkg-config --cflags --libs opencv`
OBJS          = ../drone/AtCmd.o \
                ../utils/udp.o \
                ../utils/tcp.o \
                ../cible.o \
                ../video.o \
                ../main.o
PROGRAM       = test.a

$(PROGRAM):     $(OBJS)
		$(CXX) $(OBJS) -o $(PROGRAM) $(CXXFLAGS) $(LDFLAGS) $(LIBS)

clean:;         rm -f *.o *~ $(PROGRAM) $(OBJS)

install:        $(PROGRAM)
		install -s $(PROGRAM) $(DEST)
