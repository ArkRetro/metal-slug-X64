/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0799a6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_799a6(void)

{
  ushort uVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_9f88();
  candidate_9ea8();
  candidate_4256();
  *unaff_A6 = 0x799d0;
  candidate_1abc8();
  *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(unaff_A6[3] + 0x1e);
  uVar1 = *(ushort *)((int)unaff_A6 + 0x1e);
  uVar2 = uVar1 < 2;
  *(ushort *)((int)unaff_A6 + 0x1e) = uVar1 - 2;
  candidate_427a();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_4188;
    uVar2 = 0;
  }
  candidate_19504();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_4188;
  }
  return;
}
