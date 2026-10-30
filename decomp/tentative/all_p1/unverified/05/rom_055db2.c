/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x055db2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_55db2(void)

{
  char cVar1;
  int unaff_A6;

  if (((0x30 < *(ushort *)(unaff_A6 + 0x3c)) || ((*(byte *)(unaff_A6 + 0x15) & 1) != 0)) &&
     ((*(ushort *)(unaff_A6 + 0x3c) < 0x110 || ((*(byte *)(unaff_A6 + 0x15) & 1) == 0)))) {
    cVar1 = candidate_55ec8();
    if (cVar1 != '\x02') {
      return;
    }
  }
  *(undefined1 *)(unaff_A6 + 0x94) = 0xff;
  return;
}
