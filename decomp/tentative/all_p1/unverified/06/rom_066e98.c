/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x066e98.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

ulonglong candidate_66e98(void)

{
  int *piVar1;
  char cVar2;
  int unaff_A5;

  piVar1 = *(int **)(unaff_A5 + 0x4586);
  if ((*(ushort *)(*piVar1 + 0xe) & 0xe) == 0) {
    cVar2 = '\x02';
  }
  else {
    cVar2 = '\0';
  }
  return (ulonglong)
         CONCAT14(*(char *)((int)piVar1 + 9) + cVar2 + '\x01',
                  (uint)(byte)(*(char *)(piVar1 + 2) + *(char *)((int)piVar1 + 10) + '\x01'));
}
