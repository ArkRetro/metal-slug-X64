/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03d4ac.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3d4ac(void)

{
  candidate_9e6c();
  candidate_9d8c();
  if (sRam001069e2 == 0) {
    return;
  }
  if (sRam001069e6 == 1) {
    if ((cRam001069e8 != '\f') && (cRam001069e8 = '\f', sRam001069e4 != 1)) {
      bRam001069e9 = bRam001069e9 | 0x80;
    }
  }
  else {
    sRam001069e6 = 1;
    cRam001069e8 = '\f';
    bRam001069e9 = bRam001069e9 | 0x80;
  }
  return;
}
