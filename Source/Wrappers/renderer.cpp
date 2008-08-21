#include <windows.h>
#include "datatypes.h"
#include "renderer.h"

renderer::renderer(void) : cam(0) {}

void renderer::AttachCamera(camera * _cam)
{
	if(cam)
		ReleaseCamera();
	cam = _cam;
}

void renderer::ReleaseCamera() { cam = 0; }