/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0773ac.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_773ac(void)

{
  undefined2 uVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_9f88();
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  if (*(char *)(unaff_A6 + 0x27) == '\0') {
    uVar1 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar1;
  }
  else {
    uVar1 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar1;
  }
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  candidate_4256();
  candidate_1b1dc();
  *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) + -4;
  candidate_40bc();
  candidate_7907a();
  *unaff_A6 = 0x77438;
  uVar2 = 0;
  candidate_78d96();
  candidate_1ac38();
  candidate_140c0();
  candidate_427a();
  candidate_4c24();
  if ((bool)uVar2) {
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      *(undefined1 *)((int)unaff_A6 + 0x9b) = 3;
      *unaff_A6 = candidate_77488;
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar2 = 0;
  }
  candidate_78d46();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_77e50;
  }
  return;
}
