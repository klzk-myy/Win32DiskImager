# Contributing Translations

To contribute a translation, you can either download the base English translation file (`src/lang/diskimager_en.ts`) or clone the entire repository (see [DEVEL.md](DEVEL.md)).

## Getting Qt

You need Qt Linguist to create translations. Download Qt from https://www.qt.io/download-open-source

1. Run the installer and follow the prompts
2. During component selection, select any Qt version ( Linguist is included with all of them)
3. Default installation path: `C:\Qt`

## Adding a New Translation

1. Install Qt as described above
2. Open **Qt Linguist** from the Start Menu
3. File → Open → browse to `src/lang/diskimager_en.ts`
4. Edit → Translation File Settings → set your target language and country/region
5. File → Save As → `src/lang/diskimager_??.ts` (use the ISO 639-1 language code, e.g., `ja` for Japanese)
6. Translate all the strings into your language and save the file

### Testing your translation

1. In Qt Linguist, select File → Release As
2. Save the resulting `diskimager_??.qm` file to the `Release/` directory alongside the executable
3. Launch the application — it will automatically load the translation based on your system locale

### Adding the translation to the build

Edit `src/DiskImager.pro` and add your language code to the `LANGUAGES` list:

```
LANGUAGES = es it pl nl de fr zh_CN zh_TW ta_IN xx
```

Replace `xx` with your language code. Rebuild the project to compile the `.ts` into a `.qm` binary.

## Sharing Your Translation

You can share your translation by:

- Uploading the `diskimager_??.ts` file as an attachment to a feature request on SourceForge
- Emailing a copy to the maintainers
- Forking the git repository and submitting a merge request (see [DEVEL.md](DEVEL.md) for instructions)

## Current Translations

| Language | File | Status |
|----------|------|--------|
| Spanish | `diskimager_es.ts` | Complete |
| Italian | `diskimager_it.ts` | Complete |
| Polish | `diskimager_pl.ts` | 3 untranslated |
| Dutch | `diskimager_nl.ts` | Complete |
| German | `diskimager_de.ts` | Complete |
| French | `diskimager_fr.ts` | Complete |
| Chinese (Simplified) | `diskimager_zh_CN.ts` | Complete |
| Chinese (Traditional) | `diskimager_zh_TW.ts` | Complete |
| Tamil | `diskimager_ta_IN.ts` | 57 untranslated |
