/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d6e5e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d6e5e(void)

{
  code *pcVar1;
  undefined4 *unaff_A6;
  bool bVar2;
  undefined1 uVar3;

  *(undefined1 *)(unaff_A6[3] + 0x61) = 0xff;
  *(undefined4 *)((int)unaff_A6 + 0x96) = 0;
  candidate_4256();
  *unaff_A6 = 0xd6e86;
  uVar3 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar3) {
    *unaff_A6 = 0xd6d4c;
  }
  if (*(int *)((int)unaff_A6 + 0x96) != 0) {
    candidate_49e8();
  }
  pcVar1 = *(code **)unaff_A6[3];
  if (pcVar1 == candidate_d67dc) {
    *unaff_A6 = candidate_d6ea0;
  }
  bVar2 = pcVar1 < candidate_d67dc;
  candidate_409a();
  if (!bVar2) {
    *unaff_A6 = candidate_d6ea0;
  }
  return;
}
