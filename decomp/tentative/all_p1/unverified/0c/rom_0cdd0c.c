/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0cdd0c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_cdd0c(void)

{
  byte bVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;
  bool bVar3;

  candidate_4256();
  *unaff_A6 = 0xcdd22;
  uVar2 = 0;
  candidate_ce69c();
  candidate_427a();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_cdc10;
    uVar2 = 0;
  }
  candidate_4c24();
  if ((bool)uVar2) {
    *(undefined1 *)(unaff_A6[3] + 0xa0) = 1;
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0xfff1;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xfff1;
    *(char *)(unaff_A6[3] + 0x72) = *(char *)(unaff_A6[3] + 0x72) + -1;
    candidate_406b6();
    return;
  }
  bVar1 = *(byte *)(unaff_A6[3] + 0xa0);
  if (bVar1 == 2) {
    *unaff_A6 = candidate_4188;
  }
  bVar3 = bVar1 < 2;
  candidate_409a();
  if (!bVar3) {
    *unaff_A6 = candidate_4188;
  }
  return;
}
