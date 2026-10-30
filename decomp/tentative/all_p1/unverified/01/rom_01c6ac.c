/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01c6ac.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined8 candidate_1c6ac(void)

{
  short sVar1;
  int in_D0;
  int in_D1;
  short sVar2;
  short unaff_D4w;

  sVar1 = unaff_D4w * 2;
  sVar2 = (short)((uint)((int)(short)((uint)in_D0 >> 8) * (int)*(short *)(sVar1 + 0x29fc) -
                        (int)(short)((uint)in_D1 >> 8) * (int)*(short *)(sVar1 + 0x2a7c)) >> 8);
  return CONCAT44((int)sVar2 * (int)*(short *)(sVar1 + 0x29fc) * 2 - in_D0,
                  (int)sVar2 * (int)*(short *)(sVar1 + 0x2a7c) * -2 - in_D1);
}
