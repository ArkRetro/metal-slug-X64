/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0cde76.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_cde76(void)

{
  byte bVar1;
  undefined1 uVar3;
  undefined2 uVar2;
  undefined4 extraout_A0;
  undefined4 *unaff_A6;
  bool bVar4;

  uVar3 = candidate_260c8();
  *(undefined1 *)((int)unaff_A6 + 0x72) = uVar3;
  candidate_1b04a();
  unaff_A6[0x22] = extraout_A0;
  uVar2 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x26) = uVar2;
  candidate_4256();
  *unaff_A6 = 0xcdec2;
  uVar3 = 0;
  candidate_ce6c0();
  candidate_427a();
  if ((bool)uVar3) {
    *unaff_A6 = 0xcdde0;
    *(char *)((int)unaff_A6 + 0x72) = *(char *)((int)unaff_A6 + 0x72) + -1;
    if (*(char *)((int)unaff_A6 + 0x72) < '\x01') {
      *unaff_A6 = 0xcddc6;
      *(undefined1 *)(unaff_A6[3] + 0x72) = 0;
    }
  }
  bVar1 = *(byte *)(unaff_A6[3] + 0xa0);
  if (bVar1 == 2) {
    *unaff_A6 = candidate_4188;
  }
  bVar4 = bVar1 < 2;
  candidate_4c24();
  if (bVar4) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
    *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
  }
  if ((*(byte *)(unaff_A6[3] + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_cdf54;
  }
  return;
}
