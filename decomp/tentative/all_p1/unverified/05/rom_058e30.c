/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x058e30.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_58e30(void)

{
  short sVar1;
  int extraout_A0;
  undefined4 *unaff_A6;
  bool bVar2;

  *(undefined2 *)(unaff_A6 + 0x20) = 4;
  candidate_59c2c();
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  *(undefined1 *)(unaff_A6 + 0x24) = 0xff;
  *unaff_A6 = 0x58e48;
  candidate_59214();
  bVar2 = *(ushort *)(unaff_A6 + 0x1e) < 3;
  if (*(ushort *)(unaff_A6 + 0x1e) == 3) {
    candidate_59efc();
    if (bVar2) {
      *unaff_A6 = candidate_58df6;
    }
    else {
      sVar1 = candidate_59efc();
      if ((bVar2) && (sVar1 == 0)) {
        *unaff_A6 = candidate_58df6;
      }
    }
  }
  sVar1 = *(short *)((int)unaff_A6 + 0x86);
  *(short *)((int)unaff_A6 + 0x86) = sVar1 + -1;
  if (sVar1 == 0) {
    *(undefined1 *)((int)unaff_A6 + 0x8e) = 0;
    *unaff_A6 = candidate_58efc;
  }
  if ((*(char *)((int)unaff_A6 + 0x8f) != '\0') &&
     (sVar1 = *(short *)((int)unaff_A6 + 0x7a), *(short *)((int)unaff_A6 + 0x7a) = sVar1 + -1,
     sVar1 == 0)) {
    *(undefined2 *)((int)unaff_A6 + 0x7a) = 0;
    bVar2 = false;
    candidate_1affc();
    if (!bVar2) {
      if (*(short *)(unaff_A6 + 0xf) < 0xa0) {
        if (*(short *)(extraout_A0 + 0x3c) < 0xa0) {
          *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
          *unaff_A6 = candidate_5899a;
        }
      }
      else if (0xa0 < *(short *)(extraout_A0 + 0x3c)) {
        *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
        *unaff_A6 = candidate_5899a;
      }
    }
  }
  candidate_591fe();
  candidate_5929a();
  return;
}
