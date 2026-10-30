/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0440de.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

byte candidate_440de(void)

{
  char cVar1;
  byte bVar2;

  cVar1 = candidate_1ae8e();
  if (cVar1 == '\x03') {
    bVar2 = candidate_29c6();
    bVar2 = bVar2 & 1;
  }
  else {
    bVar2 = cVar1 - 1;
  }
  return bVar2;
}
