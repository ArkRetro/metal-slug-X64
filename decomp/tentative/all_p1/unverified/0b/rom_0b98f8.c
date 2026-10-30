/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0b98f8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_b98f8(void)

{
  char cVar1;
  ushort uVar2;
  short in_D0w;
  int extraout_A0;
  int unaff_A6;
  bool bVar3;

  if (*(char *)(unaff_A6 + 0x7a) == '\0') {
    *(short *)(unaff_A6 + 0x7c) = in_D0w;
    if (in_D0w == 0) {
      cVar1 = *(char *)(unaff_A6 + 0x78);
    }
    else {
      cVar1 = *(char *)(unaff_A6 + 0x79);
    }
    bVar3 = false;
    if ((cVar1 == '\0') && (candidate_1aeb0(), bVar3)) {
      bVar3 = false;
      candidate_1af1a();
      if (!bVar3) {
        uVar2 = *(short *)(extraout_A0 + 0x3c) - *(short *)(unaff_A6 + 0x3c);
        bVar3 = uVar2 < 4;
        if ((short)uVar2 < 5) {
          candidate_1af3c();
          if (bVar3) {
            return;
          }
          if (*(short *)(unaff_A6 + 0x7c) == 0) {
            cVar1 = *(char *)(unaff_A6 + 0x76);
          }
          else {
            cVar1 = *(char *)(unaff_A6 + 0x77);
          }
          if ((cVar1 != '\0') && (*(char *)(unaff_A6 + 0x7b) != '\0')) {
            *(undefined1 *)(unaff_A6 + 0x7a) = 1;
          }
        }
        if (*(short *)(unaff_A6 + 0x7c) == 0) {
          *(undefined1 *)(unaff_A6 + 0x78) = 1;
        }
        else {
          *(undefined1 *)(unaff_A6 + 0x79) = 1;
        }
        candidate_b99ac();
        return;
      }
    }
  }
  return;
}
