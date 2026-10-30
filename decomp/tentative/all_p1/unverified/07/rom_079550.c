/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x079550.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_79550(void)

{
  ushort uVar1;
  undefined2 uVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_9f88();
  candidate_9ea8();
  candidate_4520();
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  candidate_1ad6e();
  candidate_4256();
  uVar2 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar2;
  *unaff_A6 = 0x795b2;
  candidate_1abc8();
  *(undefined1 *)((int)unaff_A6 + 0x8b) = *(undefined1 *)(unaff_A6[3] + 0x8b);
  *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(unaff_A6[3] + 0x1e);
  uVar1 = *(ushort *)((int)unaff_A6 + 0x1e);
  uVar3 = uVar1 < 8;
  *(ushort *)((int)unaff_A6 + 0x1e) = uVar1 - 8;
  candidate_44f6();
  candidate_427a();
  candidate_79e1c();
  candidate_7a040();
  candidate_7a032();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_4188;
  }
  return;
}
