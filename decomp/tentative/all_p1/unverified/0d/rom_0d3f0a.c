/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d3f0a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d3f0a(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  *(undefined2 *)(unaff_A6 + 0x1d) = 0x3c;
  *(ushort *)((int)unaff_A6 + 0xa2) = *(ushort *)((int)unaff_A6 + 0xa2) ^ 1;
  candidate_12192();
  candidate_4256();
  *unaff_A6 = 0xd3f36;
  if (*(int *)((int)unaff_A6 + 0x9e) != 0) {
    candidate_4e76();
    *(undefined4 *)((int)unaff_A6 + 0x9e) = 0;
  }
  uVar1 = 0;
  candidate_d5566();
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_d3f82;
  }
  uVar1 = 0;
  if (*(int *)((int)unaff_A6 + 0x9a) != 0) {
    candidate_49e8();
  }
  candidate_d50a8();
  candidate_d50ec();
  candidate_4c24();
  if ((bool)uVar1) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar1 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_d40a6;
    uVar1 = 0;
  }
  candidate_1945c();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_d41de;
  }
  return;
}
