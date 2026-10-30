/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x024358.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_24358(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_4256();
  *(undefined1 *)((int)unaff_A6 + 0x8a) = *(undefined1 *)(unaff_A6[3] + 0x8a);
  if (*(char *)((int)unaff_A6 + 0x8a) == '\x02') {
    *(undefined2 *)((int)unaff_A6 + 0x26) = 0x200;
  }
  else {
    *(undefined2 *)((int)unaff_A6 + 0x26) = 0x100;
  }
  *(undefined1 *)(unaff_A6 + 0x27) = 0;
  *unaff_A6 = 0x24396;
  uVar1 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar1) {
    candidate_24ab0();
    if ((bool)uVar1) {
      *unaff_A6 = candidate_24678;
      uVar1 = 0;
    }
    else {
      *unaff_A6 = candidate_243e2;
      uVar1 = 0;
    }
  }
  candidate_4c24();
  if ((bool)uVar1) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar1 = 0;
  }
  candidate_24cca();
  candidate_24ab0();
  if ((bool)uVar1) {
    unaff_A6[0x14] = 0;
  }
  return;
}
