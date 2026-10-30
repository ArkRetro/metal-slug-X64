/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03cf9a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 candidate_3cf9a(void)

{
  undefined2 uVar1;
  short in_D0w;
  undefined2 in_D1w;
  undefined1 unaff_D2b;

  uVar1 = (undefined2)
          CONCAT31((int3)(CONCAT22(in_D0w,(short)CONCAT31((int3)(CONCAT22(in_D0w,in_D1w) >> 8),
                                                          (char)in_D1w << 4) << 4) >> 8),unaff_D2b);
  _DAT_003c0000 = CONCAT22(0x8200,uVar1);
  return CONCAT22(uVar1,in_D0w + 0x20);
}
