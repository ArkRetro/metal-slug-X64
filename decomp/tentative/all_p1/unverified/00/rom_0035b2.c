/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0035b2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_35b2(void)

{
  ushort uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  undefined2 in_D0w;
  uint unaff_D7;
  char cVar6;

  uVar2 = (unaff_D7 % 36000) % 0xe10;
  uVar3 = uVar2 % 600;
  uVar4 = uVar3 % 0x3c;
  uVar5 = uVar4 % 6;
  uVar1 = (short)uVar5 + 4;
  cVar6 = (char)uVar1;
  if (9 < uVar1) {
    cVar6 = (char)uVar5 + -6;
  }
  _DAT_003c0000 =
       CONCAT31((int3)((uint)(CONCAT31((int3)((uint)(CONCAT31((int3)((uint)(CONCAT31((int3)((uint)(
                                                  CONCAT31((int3)((uint)(CONCAT31((int3)((uint)(
                                                  CONCAT31((int3)((uint)(CONCAT31((int3)(CONCAT22(
                                                  in_D0w,0x300) >> 8),(char)(unaff_D7 / 36000) + '0'
                                                  ) + 0x200000) >> 8),
                                                  (char)((unaff_D7 % 36000) / 0xe10) + '0') +
                                                  0x200000) >> 8),0x27) + 0x200000) >> 8),
                                                  (char)(uVar2 / 600) + '0') + 0x200000) >> 8),
                                                  (char)(uVar3 / 0x3c) + '0') + 0x200000) >> 8),0x22
                                                  ) + 0x200000) >> 8),(char)(uVar4 / 6) + '0') +
                             0x200000) >> 8),cVar6 + '0');
  return;
}
