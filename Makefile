CXX = g++
CXXFLAGS = -std=c++11
SRC_DIR = .
OBJ_DIR = obj
BIN_DIR = bin

RECEIVER_SOURCES = $(SRC_DIR)/Receiver.cc $(SRC_DIR)/SlidingWindow.cc $(SRC_DIR)/ReceiverMain.cc $(SRC_DIR)/SimpleHeader.cc
RECEIVER_OBJECTS = $(patsubst $(SRC_DIR)/%.cc, $(OBJ_DIR)/receiver_%.o, $(RECEIVER_SOURCES))
RECEIVER_EXECUTABLE = $(BIN_DIR)/receiver

SENDER_SOURCES = $(SRC_DIR)/Sender.cc $(SRC_DIR)/SlidingWindow.cc $(SRC_DIR)/SenderMain.cc $(SRC_DIR)/SimpleHeader.cc
SENDER_OBJECTS = $(patsubst $(SRC_DIR)/%.cc, $(OBJ_DIR)/sender_%.o, $(SENDER_SOURCES))
SENDER_EXECUTABLE = $(BIN_DIR)/sender

all: $(RECEIVER_EXECUTABLE) $(SENDER_EXECUTABLE)

$(RECEIVER_EXECUTABLE): $(RECEIVER_OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lz

$(SENDER_EXECUTABLE): $(SENDER_OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lz

$(OBJ_DIR)/receiver_%.o: $(SRC_DIR)/%.cc
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR)/sender_%.o: $(SRC_DIR)/%.cc
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean
