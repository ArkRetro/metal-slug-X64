/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x002624.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2624(void)

{
  uint in_D0;
  uint in_D1;
  char cVar1;

  if ((((in_D0 & 1 << (in_D1 & 0x1f)) != 0) &&
      (cVar1 = (&DAT_00100004)[(short)in_D1], cVar1 != '\0')) && (DAT_0010fdaf == '\x02')) {
    cVar1 = bcdAdjust(cVar1 + -1);
    (&DAT_00100004)[(short)in_D1] = cVar1;
  }
  return;
}
