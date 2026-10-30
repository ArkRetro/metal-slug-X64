/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x072bfc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_72bfc(void)

{
  char cVar2;
  ushort uVar1;
  int unaff_A6;

  cVar2 = -(*(char *)(unaff_A6 + 0x7a) != '\0');
  if (cVar2 != *(char *)(unaff_A6 + 0x7b)) {
    *(char *)(unaff_A6 + 0x7b) = cVar2;
    if (cVar2 == '\0') {
      candidate_4256();
    }
    else {
      candidate_4256();
    }
  }
  candidate_427a();
  if (*(char *)(unaff_A6 + 0x7b) == '\0') {
    uVar1 = (ushort)*(byte *)(unaff_A6 + 0x78);
  }
  else {
    uVar1 = (ushort)*(byte *)(unaff_A6 + 0x79) * 2;
  }
  *(undefined4 *)(unaff_A6 + 0x50) = *(undefined4 *)((short)(uVar1 * 4) + 0xe3494);
  return;
}
