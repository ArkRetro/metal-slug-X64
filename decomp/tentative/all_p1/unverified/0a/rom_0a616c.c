/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a616c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a616c(void)

{
  undefined4 in_D1;
  int iVar1;
  int unaff_A6;
  char cVar2;

  if (*(char *)(unaff_A6 + 0x7f) == '\0') {
    iVar1 = 0;
  }
  else {
    iVar1 = 0xb;
  }
  cVar2 = (CONCAT22((short)((uint)in_D1 >> 0x10),*(undefined2 *)(*(int *)(unaff_A6 + 0x6e) + 0x74))
          & 1 << iVar1) != 0;
  if ((*(byte *)(*(int *)(unaff_A6 + 0x6e) + 0x7c) & '\x01' << (*(char *)(unaff_A6 + 0x7f) != '\0'))
      != 0) {
    cVar2 = '\x02';
  }
  if (cVar2 != *(char *)(unaff_A6 + 0x7e)) {
    *(char *)(unaff_A6 + 0x7e) = cVar2;
    candidate_4256();
  }
  return;
}
