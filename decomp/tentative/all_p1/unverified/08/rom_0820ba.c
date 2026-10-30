/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0820ba.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_820ba(void)

{
  char cVar1;
  byte bVar2;
  int unaff_A6;

  cVar1 = candidate_2eb4();
  bVar2 = ((byte)(cVar1 + 2U) >> 2) - 0x10;
  bVar2 = ((-((char)bVar2 < '\0') ^ bVar2) + ((char)bVar2 < '\0')) - 0xb;
  if ((char)bVar2 < '\0') {
    bVar2 = 0;
  }
  if (10 < bVar2) {
    bVar2 = 10;
  }
  *(byte *)(unaff_A6 + 0x9e) = bVar2;
  return;
}
