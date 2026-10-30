/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02834a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2834a(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;
  bool bVar2;

  candidate_1b1dc();
  *(undefined2 *)((int)unaff_A6 + 0x26) = 0x100;
  candidate_4256();
  *unaff_A6 = 0x2836c;
  uVar1 = 0;
  candidate_1436a();
  candidate_427a();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_28386;
    uVar1 = 0;
  }
  candidate_4c24();
  if ((bool)uVar1) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_284f2;
    bVar2 = false;
    candidate_1b206();
    if (bVar2) {
      *unaff_A6 = candidate_2858c;
    }
  }
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  bVar2 = false;
  candidate_1945c();
  if (bVar2) {
    *unaff_A6 = candidate_28660;
  }
  return;
}
