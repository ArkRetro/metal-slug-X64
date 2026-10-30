/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02818e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2818e(void)

{
  undefined1 uVar2;
  ushort uVar1;
  undefined4 *unaff_A6;
  bool bVar3;

  candidate_1b1dc();
  candidate_120f8();
  uVar2 = candidate_260c8();
  *(undefined1 *)((int)unaff_A6 + 0x7b) = uVar2;
  uVar1 = candidate_260c8();
  *(ushort *)((int)unaff_A6 + 0x26) = ((uVar1 & 3) + 1) * 0x80;
  candidate_4256();
  *unaff_A6 = 0x281dc;
  uVar2 = 0;
  candidate_1436a();
  candidate_427a();
  if ((bool)uVar2) {
    *unaff_A6 = 0x28118;
  }
  candidate_49e8();
  candidate_4c24();
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_284f2;
    bVar3 = false;
    candidate_1b206();
    if (bVar3) {
      *unaff_A6 = candidate_2858c;
    }
  }
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  bVar3 = false;
  candidate_1945c();
  if (bVar3) {
    *unaff_A6 = candidate_28660;
  }
  return;
}
