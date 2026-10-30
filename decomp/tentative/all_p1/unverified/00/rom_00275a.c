/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x00275a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

byte candidate_275a(void)

{
  byte bVar1;
  undefined1 uVar2;
  int unaff_A5;

  uVar2 = DAT_0010f003;
  bVar1 = -(*(char *)(unaff_A5 + 0x7db7) == '\x02') |
          -(*(char *)(unaff_A5 + 0x7db7) == '\x01') |
          -(*(char *)(unaff_A5 + 0x7db6) == '\x01') | -(*(char *)(unaff_A5 + 0x7db6) == '\x02');
  if (bVar1 != 0) {
    return bVar1;
  }
  DAT_0010f003 = 0;
  if (DAT_0010f004 != 0) {
    DAT_0010f003 = uVar2;
    return DAT_0010f004;
  }
  return DAT_0010f004;
}
