/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x079a0e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_79a0e(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_9f88();
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1001;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1001;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  candidate_4256();
  *unaff_A6 = 0x79a4a;
  uVar1 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_4188;
    uVar1 = 0;
  }
  candidate_7a032();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_4188;
  }
  return;
}
