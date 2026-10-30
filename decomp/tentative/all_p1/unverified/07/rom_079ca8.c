/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x079ca8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_79ca8(void)

{
  ushort uVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_9f88();
  candidate_9ea8();
  candidate_4520();
  candidate_1ad6e();
  candidate_4256();
  *unaff_A6 = 0x79cf6;
  candidate_1abc8();
  *(undefined1 *)((int)unaff_A6 + 0x8b) = *(undefined1 *)(unaff_A6[3] + 0x8b);
  *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(unaff_A6[3] + 0x1e);
  uVar1 = *(ushort *)((int)unaff_A6 + 0x1e);
  uVar2 = 0xfff7 < uVar1;
  *(ushort *)((int)unaff_A6 + 0x1e) = uVar1 + 8;
  candidate_44f6();
  candidate_427a();
  candidate_49e8();
  if ((*(byte *)(unaff_A6[3] + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_4188;
    uVar2 = 0;
  }
  candidate_7a032();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_4188;
  }
  return;
}
