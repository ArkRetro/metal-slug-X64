/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x060eb0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_60eb0(void)

{
  short sVar1;
  char cVar2;
  int unaff_A6;

  if (*(short *)(unaff_A6 + 0x76) == 0) {
    return;
  }
  sVar1 = candidate_1b29c();
  if ((sVar1 < 0x31) &&
     (sVar1 = *(short *)(unaff_A6 + 0x76) - *(short *)(unaff_A6 + 0x7a),
     *(short *)(unaff_A6 + 0x76) = sVar1, sVar1 < 0)) {
    *(undefined2 *)(unaff_A6 + 0x76) = 0;
    return;
  }
  cVar2 = (char)*(ushort *)(unaff_A6 + 0x76);
  if ((*(ushort *)(unaff_A6 + 0x76) & 0x100) == 0) {
    cVar2 = -cVar2;
  }
  *(char *)(unaff_A6 + 0x12) = cVar2;
  if (((*(ushort *)(unaff_A6 + 0x76) & 0x100) == 0) && (*(char *)(unaff_A6 + 0x7c) == '\0')) {
    candidate_9ea8();
    *(undefined1 *)(unaff_A6 + 0x7c) = 0xff;
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) & 0xfe;
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) & 0xfd;
  }
  return;
}
