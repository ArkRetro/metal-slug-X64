/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x002e98.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined8 candidate_2e98(void)

{
  short sVar1;
  undefined4 in_D0;
  ushort in_D1w;
  short sVar2;

  sVar1 = (in_D1w >> 8) * 2;
  sVar2 = (short)((uint)in_D0 >> 8);
  return CONCAT44((int)sVar2 * (int)*(short *)(sVar1 + 0x2a7c),
                  (int)sVar2 * (int)*(short *)(sVar1 + 0x29fc));
}
