#include <sndio.h>

int main ()
{
  struct sio_hdl *sio_hdl = sio_open(SIO_DEVANY, SIO_PLAY, 0);
  sio_close(sio_hdl);
  return 0;
}
