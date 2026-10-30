/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x077fe4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_77fe4(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_9f88();
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8820;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8820;
  *(byte *)(unaff_A6[3] + 0x9c) =
       *(byte *)(unaff_A6[3] + 0x9c) | '\x01' << (*(ushort *)((int)unaff_A6 + 0x8a) & 7);
  candidate_78f6e();
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
  candidate_78f6e();
  candidate_1b1dc();
  candidate_4256();
  *unaff_A6 = 0x7803e;
  uVar1 = 0;
  candidate_78cf4();
  candidate_427a();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_78074;
    uVar1 = 0;
  }
  candidate_78fa2();
  candidate_78d54();
  if ((bool)uVar1) {
    *unaff_A6 = 0x77e5c;
    uVar1 = 0;
  }
  candidate_78d7a();
  if ((bool)uVar1) {
    *unaff_A6 = 0x77e5c;
  }
  return;
}
