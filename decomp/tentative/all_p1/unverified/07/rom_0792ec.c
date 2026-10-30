/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0792ec.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_792ec(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  *(undefined2 *)((int)unaff_A6 + 0x82) = 0x1b;
  *unaff_A6 = 0x792f8;
  candidate_1abc8();
  candidate_140c0();
  candidate_427a();
  uVar2 = *(byte *)((int)unaff_A6 + 0x8a) < 4;
  if (*(byte *)((int)unaff_A6 + 0x8a) != 4) {
    if (*(short *)((int)unaff_A6 + 0x82) == 0) {
      *(char *)(unaff_A6 + 0x23) = *(char *)(unaff_A6 + 0x23) + '\x01';
      sVar1 = *(short *)(unaff_A6 + 0x21) + -1;
      *(short *)(unaff_A6 + 0x21) = sVar1;
      if (sVar1 == 0) {
        *(undefined1 *)((int)unaff_A6 + 0x8a) = 0;
        *unaff_A6 = candidate_79228;
      }
      else {
        *(undefined1 *)((int)unaff_A6 + 0x8a) = 1;
        *unaff_A6 = 0x7926e;
      }
      uVar2 = false;
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
