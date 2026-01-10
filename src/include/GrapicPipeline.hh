#include "LogicalDevice.hh"


class GrapicPileline {
public:
	GrapicPileline();
	~GrapicPileline();
	void Init(LogicalDevice& Device, vk::Extent2D swapChainExtent);
private:

};