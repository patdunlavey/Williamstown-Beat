// $Id: taxonomy.js,v 1.1.2.2 2010/03/20 19:11:37 davereid Exp $

Drupal.verticalTabs = Drupal.verticalTabs || {};

Drupal.verticalTabs.taxonomy = function() {
  var terms = {};
  var termCount = 0;
  $('fieldset.vertical-tabs-taxonomy').find('select, input.form-text').each(function() {
    if (this.value) {
      var vocabulary = $(this).siblings('label').html();
      terms[vocabulary] = terms[vocabulary] || [];
      if ($(this).is('input.form-text')) {
        terms[vocabulary].push(this.value);
        termCount++;
      }
      else if ($(this).is('select')) {
        $(this).find('option[selected]').each(function() {
          var term = $(this).text().replace(/^\-+/, '');
          terms[vocabulary].push(term);
          termCount++;
        });
      }
    }
  });

  if (termCount) {
    var output = '';
    $.each(terms, function(vocab, vocab_terms) {
      if (output) {
        output += '<br />';
      }
      output += vocab;
      output += vocab_terms.join(', ');
    });
    return output;
  }
  else {
    return Drupal.t('No terms');
  }
}
