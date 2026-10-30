/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05112e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5112e(void)

{
  ushort uVar1;
  int unaff_A6;

  if (*(char *)(unaff_A6 + 0x9f) == '\x04') {
    *(char *)(unaff_A6 + 0xa0) = *(char *)(unaff_A6 + 0xa0) + '\x01';
    *(undefined1 *)(unaff_A6 + 0x14) =
         *(undefined1 *)(unaff_A6 + 0x19 + (int)(short)(*(byte *)(unaff_A6 + 0xa0) & 3));
    return;
  }
  if (*(char *)(unaff_A6 + 0x9f) == '\x02') {
    *(char *)(unaff_A6 + 0xa0) = *(char *)(unaff_A6 + 0xa0) + '\x01';
    uVar1 = (ushort)*(byte *)(unaff_A6 + 0xa0);
    if ((char)*(byte *)(unaff_A6 + 0xa0) < '\0') {
      *(undefined1 *)(unaff_A6 + 0xa0) = 0;
      uVar1 = 0;
    }
    *(undefined1 *)(unaff_A6 + 0x14) =
         *(undefined1 *)
          (unaff_A6 + 0x19 + (int)(short)(ushort)*(byte *)((short)(uVar1 & 3) + 0x51126));
    return;
  }
  return;
}
