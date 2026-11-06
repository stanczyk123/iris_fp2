std_msgs/Header header
float32 servo        # sudut kemudi derajat, negatif = kiri
int32 speed          # 0-255, diteruskan ESP32 ke STM32
bool enable          # false = motor berhenti