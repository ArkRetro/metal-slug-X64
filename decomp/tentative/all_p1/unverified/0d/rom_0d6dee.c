/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d6dee.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d6dee(void)

{
  code *pcVar1;
  undefined1 uVar2;
  undefined4 *unaff_A6;
  bool bVar3;

  candidate_120f8();
  *(undefined4 *)((int)unaff_A6 + 0x96) = 0xe916c;
  uVar2 = candidate_260c8();
  *(undefined1 *)((int)unaff_A6 + 0x76) = uVar2;
  candidate_4256();
  *unaff_A6 = 0xd6e26;
  uVar2 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar2) {
    candidate_4256();
    *(char *)((int)unaff_A6 + 0x76) = *(char *)((int)unaff_A6 + 0x76) + -1;
    if (*(char *)((int)unaff_A6 + 0x76) < '\x01') {
      *unaff_A6 = candidate_d6e5e;
    }
  }
  if (*(int *)((int)unaff_A6 + 0x96) != 0) {
    candidate_49e8();
  }
  pcVar1 = *(code **)unaff_A6[3];
  if (pcVar1 == candidate_d67dc) {
    *unaff_A6 = candidate_d6ea0;
  }
  bVar3 = pcVar1 < candidate_d67dc;
  candidate_409a();
  if (!bVar3) {
    *unaff_A6 = candidate_d6ea0;
  }
  return;
}
