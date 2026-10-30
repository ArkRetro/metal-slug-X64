/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x074e74.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

int candidate_74e74(void)

{
  int iVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_4256();
  candidate_c8022();
  *unaff_A6 = 0x74e90;
  uVar2 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_74eaa;
  }
  iVar1 = *(int *)((int)unaff_A6 + 0x6e);
  if (iVar1 != 0) {
    iVar1 = candidate_140c0();
  }
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 0x7fff;
  if (*(short *)(unaff_A6 + 0x10) < 0xc0) {
    *unaff_A6 = candidate_41ac;
  }
  if (0x240 < *(short *)(unaff_A6 + 0x10)) {
    *unaff_A6 = candidate_41ac;
  }
  return iVar1;
}
