/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07926a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7926a(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_7a0dc();
  *(undefined2 *)((int)unaff_A6 + 0x82) = 0x20;
  *(undefined2 *)(unaff_A6 + 0x20) = 0;
  *unaff_A6 = 0x79280;
  candidate_1abc8();
  candidate_140c0();
  candidate_427a();
  uVar2 = *(byte *)((int)unaff_A6 + 0x8a) < 4;
  if (*(byte *)((int)unaff_A6 + 0x8a) != 4) {
    uVar2 = false;
    if (*(short *)((int)unaff_A6 + 0x82) == 0) {
      candidate_7a0a4();
      if ((bool)uVar2) {
        if ((*(byte *)(unaff_A6 + 0x23) & 1) == 0) {
          *(undefined1 *)((int)unaff_A6 + 0x8a) = 2;
        }
        else {
          *(undefined1 *)((int)unaff_A6 + 0x8a) = 3;
        }
        *unaff_A6 = candidate_792ec;
        uVar2 = 0;
      }
    }
    else {
      sVar1 = *(short *)((int)unaff_A6 + 0x82);
      uVar2 = sVar1 == 0;
      *(short *)((int)unaff_A6 + 0x82) = sVar1 + -1;
    }
  }
  candidate_79c2e();
  candidate_7a032();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_79a70;
  }
  return;
}
