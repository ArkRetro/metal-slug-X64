/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0b9876.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_b9876(void)

{
  char cVar1;
  short in_D0w;
  int extraout_A0;
  int unaff_A6;
  bool bVar2;

  *(short *)(unaff_A6 + 0x7c) = in_D0w;
  if (in_D0w == 0) {
    cVar1 = *(char *)(unaff_A6 + 0x78);
  }
  else {
    cVar1 = *(char *)(unaff_A6 + 0x79);
  }
  if (cVar1 == '\0') {
    bVar2 = false;
    candidate_1aeb0();
    if (bVar2) {
      if ((((*(byte *)(extraout_A0 + 0x15) & 0x40) != 0) &&
          ((short)(*(short *)(extraout_A0 + 0x3c) - *(short *)(unaff_A6 + 0x3c)) < -0x24)) &&
         (-8 < (short)(*(short *)(extraout_A0 + 0x40) - *(short *)(unaff_A6 + 0x40)))) {
        if (*(short *)(unaff_A6 + 0x7c) == 0) {
          *(undefined1 *)(unaff_A6 + 0x76) = 1;
        }
        else {
          *(undefined1 *)(unaff_A6 + 0x77) = 1;
        }
        return;
      }
      return;
    }
  }
  if (*(short *)(unaff_A6 + 0x7c) == 0) {
    *(undefined1 *)(unaff_A6 + 0x76) = 0;
  }
  else {
    *(undefined1 *)(unaff_A6 + 0x77) = 0;
  }
  return;
}
