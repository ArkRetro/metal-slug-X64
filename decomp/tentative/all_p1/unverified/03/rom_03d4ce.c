/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03d4ce.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3d4ce(void)

{
  candidate_3d4ac();
  candidate_9d8c();
  if (sRam001078d2 == 0) {
    return;
  }
  if (sRam001078d6 == 1) {
    if ((cRam001078d8 != '\f') && (cRam001078d8 = '\f', sRam001078d4 != 1)) {
      bRam001078d9 = bRam001078d9 | 0x80;
    }
  }
  else {
    sRam001078d6 = 1;
    cRam001078d8 = '\f';
    bRam001078d9 = bRam001078d9 | 0x80;
  }
  return;
}
