/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0cdb62.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_cdb62(void)

{
  byte bVar1;
  undefined2 uVar2;
  undefined4 extraout_A0;
  undefined4 *unaff_A6;
  bool bVar3;

  candidate_1ad6e();
  candidate_1b04a();
  unaff_A6[0x22] = extraout_A0;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x801;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x801;
  candidate_9ea8();
  uVar2 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x26) = uVar2;
  candidate_4256();
  *unaff_A6 = 0xcdbba;
  candidate_ce69c();
  candidate_427a();
  *(short *)(unaff_A6 + 0x26) = *(short *)(unaff_A6 + 0x26) + -1;
  if (*(short *)(unaff_A6 + 0x26) < 1) {
    *unaff_A6 = candidate_cdc10;
  }
  bVar3 = false;
  candidate_4c24();
  if (bVar3) {
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
