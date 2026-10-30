/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0794f4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_794f4(void)

{
  ushort uVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_4256();
  *unaff_A6 = 0x7950a;
  candidate_1abc8();
  *(undefined1 *)((int)unaff_A6 + 0x8b) = *(undefined1 *)(unaff_A6[3] + 0x8b);
  *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(unaff_A6[3] + 0x1e);
  uVar1 = *(ushort *)((int)unaff_A6 + 0x1e);
  uVar2 = 0xfff7 < uVar1;
  *(ushort *)((int)unaff_A6 + 0x1e) = uVar1 + 8;
  candidate_44f6();
  candidate_427a();
  if ((bool)uVar2) {
    *unaff_A6 = 0x793a6;
    uVar2 = 0;
  }
  candidate_79e06();
  candidate_7a032();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_4188;
  }
  return;
}
