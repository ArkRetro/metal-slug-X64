/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x079718.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_79718(void)

{
  ushort uVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_4256();
  *unaff_A6 = 0x7972e;
  candidate_1abc8();
  *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(unaff_A6[3] + 0x1e);
  uVar1 = *(ushort *)((int)unaff_A6 + 0x1e);
  uVar2 = uVar1 < 8;
  *(ushort *)((int)unaff_A6 + 0x1e) = uVar1 - 8;
  candidate_427a();
  if ((bool)uVar2) {
    *unaff_A6 = 0x7961a;
    uVar2 = 0;
  }
  candidate_4c24();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_5db18;
    uVar2 = 0;
  }
  candidate_79e06();
  candidate_7a032();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_4188;
  }
  return;
}
